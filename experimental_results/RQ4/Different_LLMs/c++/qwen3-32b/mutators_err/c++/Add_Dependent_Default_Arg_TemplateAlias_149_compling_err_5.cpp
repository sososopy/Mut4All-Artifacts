#pragma once
#include "Mutator_base.h"

/**
 * Add_Dependent_Default_Arg_TemplateAlias_149
 */ 
class MutatorFrontendAction_149 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(149)

private:
    class MutatorASTConsumer_149 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_149(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "Add_Dependent_Default_Arg_TemplateAlias_149.h"

// ========================================================================================================
#define MUT149_OUTPUT 1

void MutatorFrontendAction_149::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *tparam = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("tparam")) {
        // Filter nodes in header files
        if (!Result.Context->getSourceManager().isWrittenInMainFile(tparam->getLocation()))
            return;

        // Check if the parent is an alias template
        if (auto *templateDecl = dyn_cast<clang::TemplateDecl>(tparam->getDeclContext())) {
            if (templateDecl->isAliasTemplate()) {
                // Check if the default argument exists and is non-dependent
                if (tparam->hasDefaultArgument() && !tparam->getDefaultArgument()->isDependentType()) {
                    // Get the source location for insertion
                    SourceLocation insertLoc = tparam->getLocation();
                    // Insert the new parameter before the existing one
                    Rewrite.InsertText(insertLoc, "typename U, ", true, true);

                    // Replace the default argument with a dependent type
                    clang::Type *defaultArg = tparam->getDefaultArgument();
                    if (defaultArg) {
                        clang::TypeSourceInfo *defaultArgLoc = defaultArg->getTypeSourceInfo();
                        if (defaultArgLoc) {
                            SourceRange argRange = defaultArgLoc->getTypeLoc().getSourceRange();
                            Rewrite.ReplaceText(argRange, "typename U::type");
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_149::MutatorASTConsumer_149::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define the AST matcher to find template type parameters with non-dependent default arguments in alias templates
    DeclarationMatcher matcher = templateDecl(
        isAliasTemplate(),
        has(templateTypeParmDecl(
            hasDefaultArgument()))
    ).bind("tparam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}