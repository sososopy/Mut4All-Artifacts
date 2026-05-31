//header file
#pragma once
#include "Mutator_base.h"

/**
 * Deduce_Static_Member_Type_429
 */ 
class MutatorFrontendAction_429 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(429)

private:
    class MutatorASTConsumer_429 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_429(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Deduce_Static_Member_Type_429.h"

// ========================================================================================================
#define MUT429_OUTPUT 1

void MutatorFrontendAction_429::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        // Filter nodes in header files
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                CTD->getLocation()))
            return;

        for (auto *spec : CTD->specializations()) {
            if (spec->isCompleteDefinition()) {
                for (auto *decl : spec->decls()) {
                    if (auto *varDecl = llvm::dyn_cast<clang::VarDecl>(decl)) {
                        if (varDecl->isStaticDataMember()) {
                            // Get the source code text of target node
                            auto typeStr = varDecl->getType().getAsString();
                            std::string newType = "decltype(" + typeStr + "{});";
                            // Perform mutation on the source code text by applying string replacement
                            Rewrite.ReplaceText(varDecl->getTypeSourceInfo()->getTypeLoc().getSourceRange(), newType);
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_429::MutatorASTConsumer_429::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl(
        hasDescendant(varDecl(isStaticDataMember(), hasType(dependentType())))
    ).bind("ClassTemplate");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}