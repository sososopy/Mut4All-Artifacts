//source file
#include "../include/replace_constexpr_with_non_constant_expression_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ICE = Result.Nodes.getNodeAs<clang::IfStmt>("IfConstexpr")) {
        //Filter nodes in header files
        if (!ICE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ICE->getBeginLoc()))
            return;

        //Get the source code text of target node
        auto conditionExpr = ICE->getCond();
        if (conditionExpr) {
            //Perform mutation on the source code text by applying string replacement
            auto conditionStr = stringutils::rangetoStr(*(Result.SourceManager),
                                                        conditionExpr->getSourceRange());
            std::string mutatedCondition = "undeclaredValue" + conditionStr.substr(conditionStr.find_first_of("><=!"));
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(conditionExpr->getSourceRange()), mutatedCondition);
        }
    }
}
  
void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = ifStmt(isConstexpr()).bind("IfConstexpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}