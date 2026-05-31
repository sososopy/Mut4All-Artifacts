//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_alias_incomplete_type_521
 */ 
class MutatorFrontendAction_521 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(521)

private:
    class MutatorASTConsumer_521 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_521(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_alias_incomplete_type_521.h"

// ========================================================================================================
#define MUT521_OUTPUT 1

void MutatorFrontendAction_521::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AliasDecl = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
        if (!AliasDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                AliasDecl->getLocation()))
            return;

        if (auto *TypeAliasTemplate = dyn_cast<TypeAliasTemplateDecl>(AliasDecl->getDeclContext())) {
            std::string aliasName = AliasDecl->getNameAsString();
            std::string incompleteType = "struct IncompleteType;";
            std::string replacement = incompleteType + "\nconstexpr " + aliasName + " tmp = " + aliasName + "<IncompleteType>{};";
            SourceLocation insertLoc = AliasDecl->getEndLoc().getLocWithOffset(1);
            Rewrite.InsertTextAfter(insertLoc, "\n/*mut521*/" + replacement);
        }
    }
}

void MutatorFrontendAction_521::MutatorASTConsumer_521::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasDecl(hasType(templateSpecializationType())).bind("Alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}