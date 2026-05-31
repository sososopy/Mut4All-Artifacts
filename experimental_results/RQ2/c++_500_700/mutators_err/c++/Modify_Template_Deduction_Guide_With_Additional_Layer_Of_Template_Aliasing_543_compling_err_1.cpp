//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_deduction_guide_with_additional_layer_of_template_aliasing_543
 */ 
class MutatorFrontendAction_543 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(543)

private:
    class MutatorASTConsumer_543 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_543(Rewriter &R) : TheRewriter(R) {}
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
        const clang::TypeAliasDecl *OriginalAlias = nullptr;
    };
};

//source file
#include "../include/modify_template_deduction_guide_with_additional_layer_of_template_aliasing_543.h"

// ========================================================================================================
#define MUT543_OUTPUT 1

void MutatorFrontendAction_543::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Alias = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("AliasTemplate")) {
        //Filter nodes in header files
        if (!Alias || !Result.Context->getSourceManager().isWrittenInMainFile(
                Alias->getLocation()))
            return;

        OriginalAlias = Alias;

        //Get the source code text of target node
        auto aliasName = Alias->getNameAsString();
        auto newAliasName = "New" + aliasName;

        //Perform mutation on the source code text by applying string replacement
        std::string newAliasDecl = "template <class T, class... Ts>\nusing " + newAliasName + " = " + aliasName + "<T, Ts...>;\n";
        Rewrite.InsertTextAfter(Alias->getEndLoc(), "\n/*mut543*/" + newAliasDecl);

        // Replace usages of the original alias with the new alias
        for (const auto &Use : Result.Context->getTranslationUnitDecl()->decls()) {
            if (const auto *CallExpr = llvm::dyn_cast<clang::CallExpr>(Use)) {
                if (const auto *Callee = CallExpr->getCalleeDecl()) {
                    if (Callee->getNameAsString() == aliasName) {
                        auto callRange = CallExpr->getSourceRange();
                        Rewrite.ReplaceText(callRange, newAliasName + "(" + CallExpr->getArg(0)->getSourceRange().printToString(*Result.SourceManager) + ")");
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_543::MutatorASTConsumer_543::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl().bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}