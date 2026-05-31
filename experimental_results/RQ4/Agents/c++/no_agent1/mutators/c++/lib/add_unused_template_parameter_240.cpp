//source file
#include "../include/add_unused_template_parameter_240.h"

// ========================================================================================================
#define MUT240_OUTPUT 1

void MutatorFrontendAction_240::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        if (modifiedClasses.find(CD) != modifiedClasses.end())
            return;
        if (!CD->isCompleteDefinition())
            return;

        // Get the source code text of target node
        auto classText = stringutils::rangetoStr(*(Result.SourceManager), CD->getSourceRange());
        
        // Perform mutation on the source code text by applying string replacement
        std::string templateParam = "template<typename UnusedParam> ";
        classText.insert(0, templateParam);
        
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), classText);
        
        modifiedClasses.insert(CD);
    }
}

void MutatorFrontendAction_240::MutatorASTConsumer_240::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}