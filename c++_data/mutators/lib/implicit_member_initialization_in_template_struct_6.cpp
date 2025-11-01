//source file
#include "../include/Implicit_Member_Initialization_In_Template_Struct_6.h"

// ========================================================================================================
#define MUT6_OUTPUT 1

void MutatorFrontendAction_6::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("memberVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
            return;

        if (auto *Init = VD->getInit()) {
            if (auto *DRE = dyn_cast<DeclRefExpr>(Init)) {
                if (DRE->getDecl() == VD) {
                    std::string replacement = "0";
                    Rewrite.ReplaceText(VD->getInit()->getSourceRange(), replacement);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_6::MutatorASTConsumer_6::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl(
        hasParent(recordDecl(hasParent(functionDecl(isTemplateInstantiation())))),
        hasInitializer(expr().bind("init")),
        unless(isExpansionInSystemHeader())
    ).bind("memberVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}