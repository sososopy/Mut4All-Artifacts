//source file
#include "../include/Misuse_Explicit_Object_Parameters_In_Lambdas_386.h"

// ========================================================================================================
#define MUT386_OUTPUT 1

void MutatorFrontendAction_386::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
        //Filter nodes in header files
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       LE->getBeginLoc()))
            return;
        
        //Get the source code text of target node
        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                    LE->getSourceRange());
        
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedLambda = lambdaSource;
        auto paramStart = mutatedLambda.find('(');
        if (paramStart != std::string::npos) {
            mutatedLambda.insert(paramStart + 1, "this auto& obj, ");
            auto bodyStart = mutatedLambda.find('{');
            if (bodyStart != std::string::npos) {
                mutatedLambda.insert(bodyStart + 1, "return obj(a); ");
            }
        }
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), mutatedLambda);
    }
}

void MutatorFrontendAction_386::MutatorASTConsumer_386::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}