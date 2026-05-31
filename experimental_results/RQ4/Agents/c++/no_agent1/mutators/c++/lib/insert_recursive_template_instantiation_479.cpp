//source file
#include "../include/insert_recursive_template_instantiation_479.h"

// ========================================================================================================
#define MUT479_OUTPUT 1

void MutatorFrontendAction_479::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
            return;

        auto templateName = TD->getNameAsString();
        if (processedTemplates.find(templateName) != processedTemplates.end())
            return;

        processedTemplates.insert(templateName);

        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TD->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string recursiveTemplate = "\n/*mut479*/template<int N> struct " + templateName + " { " +
                                        "static const int value = " + templateName + "<N-1>::value; }; " +
                                        "template<> struct " + templateName + "<0> { static const int value = 0; };";

        declaration += recursiveTemplate;

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_479::MutatorASTConsumer_479::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}