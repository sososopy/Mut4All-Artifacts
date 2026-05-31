//source file
#include "../include/insert_unreachable_code_57.h"

// ========================================================================================================
#define MUT57_OUTPUT 1

void MutatorFrontendAction_57::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        //Filter nodes in header files
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(FS->getLocation()))
            return;

        if (FS->hasBody()) {
            //Get the source code text of target node
            auto body = stringutils::rangetoStr(*(Result.SourceManager), FS->getBody()->getSourceRange());
            //Perform mutation on the source code text by applying string replacement
            std::string unreachableCode = "\n/*mut57*/if (false) { int unreachable = 0; }\n";
            body.insert(body.find("{") + 1, unreachableCode);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getBody()->getSourceRange()), body);
        }
    }
}

void MutatorFrontendAction_57::MutatorASTConsumer_57::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}