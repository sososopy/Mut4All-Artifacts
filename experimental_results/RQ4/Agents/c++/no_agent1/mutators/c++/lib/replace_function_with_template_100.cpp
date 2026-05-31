//source file
#include "../include/replace_function_with_template_100.h"

// ========================================================================================================
#define MUT100_OUTPUT 1

void MutatorFrontendAction_100::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        
        // Ensure the function is not a template and has a body
        if (FD->isTemplateInstantiation() || !FD->hasBody())
            return;

        //Get the source code text of target node
        auto functionText = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FD->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string templateText = "template<typename T>\n";
        functionText.insert(0, templateText);

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
    }
}
  
void MutatorFrontendAction_100::MutatorASTConsumer_100::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(unless(isTemplateInstantiation())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}