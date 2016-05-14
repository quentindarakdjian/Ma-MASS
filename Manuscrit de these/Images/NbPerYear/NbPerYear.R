# Number of publication per year
# Year | Nb publi

publi <- read.csv("//AIEDISK/AI Environnement/PROJETS/25 R&D/Manuscrit de these/Images/NbPerYear/publi.csv", sep=";",header=TRUE)
plot(publi$Année, publi$Nb.publication, main="Number of identified publications by  per year ", xlab="Year", ylab="Number of publications", cex.main=1)
panel.smooth(publi$Année, publi$Nb.publication, col.smooth = "red", span = 0.5)
