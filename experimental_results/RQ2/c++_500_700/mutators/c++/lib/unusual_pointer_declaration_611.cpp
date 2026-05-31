//source file
#include "../include/unusual_pointer_declaration_611.h"

// ========================================================================================================
#define MUT611_OUTPUT 1

void MutatorFrontendAction_611::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("pointerDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        
        if (VD->getType()->isPointerType()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
            std::string mutatedDeclaration = declaration + ", **ptr2, ***ptr3;";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), mutatedDeclaration);
        }
    }
}
  
void MutatorFrontendAction_611::MutatorASTConsumer_611::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(pointerType())).bind("pointerDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}