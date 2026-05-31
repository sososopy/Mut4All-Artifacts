//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_argument_usage_in_alias_templates_548
 */ 
class MutatorFrontendAction_548 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(548)

private:
    class MutatorASTConsumer_548 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_548(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/modify_template_argument_usage_in_alias_templates_548.h"

// ========================================================================================================
#define MUT548_OUTPUT 1

void MutatorFrontendAction_548::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *AliasDecl = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("AliasTemplate")) {
        // Filter nodes in header files
        if (!AliasDecl || !Result.Context->getSourceManager().isWrittenInMainFile(AliasDecl->getLocation()))
            return;

        // Get the source code text of target node
        auto aliasSource = stringutils::rangetoStr(*(Result.SourceManager), AliasDecl->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string mutatedSource = aliasSource;
        size_t pos = mutatedSource.find("std::integral_constant<int, 5>");
        if (pos != std::string::npos) {
            mutatedSource.replace(pos, std::string("std::integral_constant<int, 5>").length(), "std::integral_constant<int, 1 + 2>");
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(AliasDecl->getSourceRange()), mutatedSource);
    }
}

void MutatorFrontendAction_548::MutatorASTConsumer_548::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = typeAliasDecl(hasType(asString("std::integral_constant<int, 5>"))).bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}