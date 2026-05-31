//source file
#include "../include/template_constraint_violation_in_lambda_61.h"

// ========================================================================================================
#define MUT61_OUTPUT 1

void MutatorFrontendAction_61::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("templateFunction")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        CurrentFunction = FD;

        for (const auto *Body : FD->getBody()->children()) {
            if (const auto *LambdaExpr = llvm::dyn_cast<clang::LambdaExpr>(Body)) {
                const auto *CallOperator = LambdaExpr->getCallOperator();
                if (CallOperator && CallOperator->getNumParams() == 1) {
                    const auto *ParamType = CallOperator->getParamDecl(0)->getType().getTypePtr();
                    
                    std::string OriginalTypeStr = ParamType->getCanonicalTypeInternal().getAsString();
                    std::string MutatedTypeStr = "unsigned int";

                    if (OriginalTypeStr != MutatedTypeStr) {
                        std::string LambdaCall = "/*mut61*/" + FD->getNameAsString() + "(" + OriginalTypeStr + "())(" + MutatedTypeStr + "(0));";
                        SourceLocation InsertLoc = FD->getBodyRBrace();
                        Rewrite.InsertTextBefore(InsertLoc, LambdaCall);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_61::MutatorASTConsumer_61::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), hasDescendant(lambdaExpr())).bind("templateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}