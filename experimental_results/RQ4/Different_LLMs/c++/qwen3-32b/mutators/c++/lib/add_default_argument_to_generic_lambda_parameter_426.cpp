//source file
#include "../include/Add_Default_Argument_To_Generic_Lambda_Parameter_426.h"

// ========================================================================================================
#define MUT426_OUTPUT 1

void MutatorFrontendAction_426::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *param = Result.Nodes.getNodeAs<clang::ParmVarDecl>("param")) {
        if (!param || !Result.Context->getSourceManager().isWrittenInMainFile(param->getLocation()))
            return;

        if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
            if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getBeginLoc()))
                return;

            SourceLocation nameLoc = param->getLocation();
            Rewrite.InsertTextAfter(nameLoc, " = 0");
        }
    }
}
  
void MutatorFrontendAction_426::MutatorASTConsumer_426::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = parmVarDecl(
        hasType(autoType()),
        unless(hasDefaultArgument()),
        hasAncestor(lambdaExpr().bind("lambda"))
    ).bind("param");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}