//source file
#include "../include/partial_specialization_unused_template_parameters_563.h"

// ========================================================================================================
#define MUT563_OUTPUT 1

void MutatorFrontendAction_563::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
            return;

        // Get the source code text of target node
        std::string className = TD->getNameAsString();
        if (specializedClasses.find(className) != specializedClasses.end())
            return;

        specializedClasses.insert(className);

        // Perform mutation on the source code text by applying string replacement
        std::string specialization = "template<typename T, typename U>\nclass " + className + "<T, U*> {\n"
                                     "    void someFunction() {\n"
                                     "        // No reference to 'U' in this specialization\n"
                                     "    }\n"
                                     "};\n";

        // Replace the original AST node with the mutated one
        SourceLocation insertLoc = TD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, specialization);
    }
}
  
void MutatorFrontendAction_563::MutatorASTConsumer_563::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}