//source file
#include "../include/insert_unused_template_parameter_406.h"

// ========================================================================================================
#define MUT406_OUTPUT 1

void MutatorFrontendAction_406::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CD->getLocation()))
            return;
        if (!CD->isCompleteDefinition() || CD->isLambda())
            return;
        
        if (visitedClasses.find(CD) != visitedClasses.end())
            return;
        
        visitedClasses.insert(CD);
        
        // Get the source code text of target node
        auto className = CD->getNameAsString();
        if (className.empty())
            return;

        // Perform mutation on the source code text by applying string replacement
        std::string templateParam = "template<typename UnusedParam>\n";
        std::string classDef = stringutils::rangetoStr(*(Result.SourceManager), CD->getSourceRange());
        classDef.insert(0, templateParam);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), classDef);
    }
}

void MutatorFrontendAction_406::MutatorASTConsumer_406::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(unless(isTemplateInstantiation())).bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}