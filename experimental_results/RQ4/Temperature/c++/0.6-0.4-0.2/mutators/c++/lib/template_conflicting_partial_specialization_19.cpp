//source file
#include "../include/template_conflicting_partial_specialization_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
            return;

        auto templateName = CT->getNameAsString();
        std::string specialization1 = "template<> class " + templateName + "<int> {};\n";
        std::string specialization2 = "template<class T> class " + templateName + "<T*> {};\n";

        SourceLocation insertLocation = CT->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLocation, "/*mut19*/\n" + specialization1 + specialization2, true, true);
    }
}

void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}