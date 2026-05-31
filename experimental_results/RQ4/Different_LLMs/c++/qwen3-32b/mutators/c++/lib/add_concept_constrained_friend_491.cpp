//source file
#include "../include/Add_Concept_Constrained_Friend_491.h"

// ========================================================================================================
#define MUT491_OUTPUT 1

void MutatorFrontendAction_491::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FriendDecl>("friendDecl")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        // Check if there are available concepts
        if (availableConcepts.empty())
            return;

        // Select a random concept
        const auto *concept = availableConcepts[0]; // For simplicity, pick the first one

        // Get the source code of the friend declaration
        std::string friendCode = stringutils::rangetoStr(*Result.SourceManager, FD->getSourceRange());

        // Construct the mutated code
        std::string conceptName = concept->getNameAsString();
        std::string param = "U"; // Template parameter name
        std::string templateClause = "template <" + conceptName + " " + param + "> ";
        std::string mutatedCode = templateClause + friendCode;

        // Replace the original code
        Rewrite.ReplaceText(FD->getSourceRange(), mutatedCode);
    } else if (auto *D = Result.Nodes.getNodeAs<clang::Decl>("conceptDecl")) {
        // Collect concepts
        if (isa<clang::ConceptDecl>(D))
            availableConcepts.push_back(cast<clang::ConceptDecl>(D));
    }
}

void MutatorFrontendAction_491::MutatorASTConsumer_491::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    // Matcher for friend declarations inside class templates
    DeclarationMatcher friendMatcher = classTemplateDecl(
        hasDescendant(friendDecl().bind("friendDecl"))
    );

    // Matcher for concepts using decl() instead of conceptDecl()
    DeclarationMatcher conceptMatcher = clang::ast_matchers::decl().bind("conceptDecl");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(friendMatcher, &callback);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.matchAST(Context);
}