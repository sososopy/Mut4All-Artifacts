//source file
#include "../include/Transform_Function_To_Template_And_Add_Reference_39.h"

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation())
            return;

        std::string funcText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        if (!FD->isFunctionTemplateSpecialization()) {
            funcText.insert(funcText.find(FD->getNameAsString()), "template<typename T> ");
        }

        funcText.insert(funcText.find("{") + 1, "\nA::S localS; int &refMember = localS.member; T x; x.someMethod(refMember);\n");

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcText);
    }
}
  
void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasParent(namespaceDecl(hasName("B")))).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}