//source file
#include "../include/insert_recursive_template_instantiation_150.h"

// ========================================================================================================
#define MUT150_OUTPUT 1

void MutatorFrontendAction_150::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TD->getLocation()))
            return;
        
        if (visitedTemplates.find(TD) != visitedTemplates.end())
            return;

        visitedTemplates.insert(TD);

        //Get the source code text of target node
        auto templateName = TD->getNameAsString();
        std::string recursiveInstantiation = "template<> class " + templateName + "<" + templateName + "<0>>; /*mut150*/\n";

        //Perform mutation on the source code text by applying string replacement
        SourceLocation insertLoc = TD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, recursiveInstantiation, true, true);
    }
}

void MutatorFrontendAction_150::MutatorASTConsumer_150::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}