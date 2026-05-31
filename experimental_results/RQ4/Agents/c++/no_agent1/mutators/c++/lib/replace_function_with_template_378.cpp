//source file
#include "../include/replace_function_with_template_378.h"

// ========================================================================================================
#define MUT378_OUTPUT 1

void MutatorFrontendAction_378::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        
        if (!FD->isThisDeclarationADefinition() || FD->isTemplateInstantiation())
            return;

        // Get the source code text of target node
        auto functionText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string templateText = "template<typename T>\n";
        functionText.insert(0, templateText);
        functionText.replace(functionText.find(FD->getNameAsString()), FD->getNameAsString().length(), "function_template");

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);

        functions.push_back(FD);
    }
}
  
void MutatorFrontendAction_378::MutatorASTConsumer_378::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}