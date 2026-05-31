//source file
#include "../include/modify_template_usage_in_aliased_type_30.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TemplateAlias")) {
        //Filter nodes in header files
        if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
            return;

        //Get the source code text of target node
        auto aliasDecl = TA->getTemplatedDecl();
        if (!aliasDecl)
            return;

        auto aliasType = aliasDecl->getUnderlyingType().getAsString();
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedType = aliasType + "<NonExistentType<T>>";
        std::string originalAlias = aliasDecl->getNameAsString() + " = " + aliasType;
        std::string mutatedAlias = aliasDecl->getNameAsString() + " = " + mutatedType;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(aliasDecl->getSourceRange(), mutatedAlias);
    }
}

void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("TemplateAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}