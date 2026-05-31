//source file
#include "../include/lambda_auto_initializer_674.h"

// ========================================================================================================
#define MUT674_OUTPUT 1

void MutatorFrontendAction_674::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("autoVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (VD->hasInit() && VD->getType()->isDependentType()) {
            auto initExpr = VD->getInit();
            std::string initText = Lexer::getSourceText(CharSourceRange::getTokenRange(initExpr->getSourceRange()), 
                                                        *Result.SourceManager, 
                                                        Result.Context->getLangOpts()).str();
            std::string lambdaInit = "[]() { return " + initText + "; }()";

            Rewrite.ReplaceText(initExpr->getSourceRange(), lambdaInit);
        }
    }
}
  
void MutatorFrontendAction_674::MutatorASTConsumer_674::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(autoType()), hasInitializer(expr())).bind("autoVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}