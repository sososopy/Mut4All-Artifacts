//source file
#include "../include/Mutator_359.h"

// ========================================================================================================
#define MUT359_OUTPUT 1

void MutatorFrontendAction_359::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TypeAliasTemplate")) {
        //Filter nodes in header files
        if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(TA->getLocation()))
            return;

        //Get the source code text of target node
        auto aliasName = TA->getNameAsString();
        auto templateParam = TA->getTemplateParameters()->getParam(0)->getNameAsString();

        //Perform mutation on the source code text by applying string replacement
        std::string structDef = "struct " + aliasName + " { " + templateParam + " value; };";
        std::string newAlias = "using " + aliasName + " = " + structDef;

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(TA->getSourceRange(), newAlias);
    }
}

void MutatorFrontendAction_359::MutatorASTConsumer_359::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("TypeAliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}