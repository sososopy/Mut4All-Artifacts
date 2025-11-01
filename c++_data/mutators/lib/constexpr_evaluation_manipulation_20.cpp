//source file
#include "../include/Constexpr_Evaluation_Manipulation_20.h"

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("ConstexprFunction")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;
        
        if (FD->isConstexpr() && FD->hasBody()) {
            //Get the source code text of target node
            auto functionBody = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());
            //Perform mutation on the source code text by applying string replacement
            std::string mutatedBody = "\nif (n == 0) return 1;\nreturn n * " + FD->getNameAsString() + "(n - 1 + (n % 2));\n";
            functionBody = "{ /*mut20*/" + mutatedBody + "}";
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBody);
        }
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ConstexprVariable")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
            return;

        if (VD->isConstexpr()) {
            //Get the source code text of target node
            auto varInit = stringutils::rangetoStr(*(Result.SourceManager), VD->getInit()->getSourceRange());
            //Perform mutation on the source code text by applying string replacement
            std::string mutatedInit = "(5 << 1) + (8 / 2) - (3 & 1)";
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getInit()->getSourceRange()), mutatedInit);
        }
    }
}
  
void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher functionMatcher = functionDecl(isConstexpr()).bind("ConstexprFunction");
    DeclarationMatcher variableMatcher = varDecl(isConstexpr()).bind("ConstexprVariable");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(variableMatcher, &callback);
    matchFinder.matchAST(Context);
}