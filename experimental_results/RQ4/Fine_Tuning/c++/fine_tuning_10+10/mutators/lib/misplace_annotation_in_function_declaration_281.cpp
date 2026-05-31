//source file
#include "../include/misplace_annotation_in_function_declaration_281.h"

// ========================================================================================================
#define MUT281_OUTPUT 1

void MutatorFrontendAction_281::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                        FD->getLocation()))
            return;

        //Get the source code text of target node
        auto functionDeclText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string annotation = "[[unknown_annotation]] ";
        if (functionDeclText.find("int") != std::string::npos) {
            functionDeclText.insert(functionDeclText.find("int") + 3, annotation);
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionDeclText);
    }
}
  
void MutatorFrontendAction_281::MutatorASTConsumer_281::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(returns(asString("int"))).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}