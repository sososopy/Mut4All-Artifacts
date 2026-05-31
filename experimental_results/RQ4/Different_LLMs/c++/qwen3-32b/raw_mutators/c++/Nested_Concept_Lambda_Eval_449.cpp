//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Concept_Lambda_Eval_449
 */ 
class MutatorFrontendAction_449 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(449)

private:
    class MutatorASTConsumer_449 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_449(Rewriter &R) : TheRewriter(R) {}
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
        std::string conceptCName;
        std::string conceptDName;
        bool foundC = false;
        bool foundD = false;
        bool inserted = false;
    };
};

//source file
#include "../include/Nested_Concept_Lambda_Eval_449.h"

// ========================================================================================================
#define MUT449_OUTPUT 1

void MutatorFrontendAction_449::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *conceptDecl = Result.Nodes.getNodeAs<clang::TemplateDecl>("concept")) {
        if (!conceptDecl || !Result.Context->getSourceManager().isWrittenInMainFile(conceptDecl->getLocation()))
            return;

        if (conceptDecl->isConcept()) {
            auto *conceptDef = conceptDecl->getTemplatedDecl();
            if (auto *conceptDefStmt = dyn_cast<clang::ConceptDecl>(conceptDef)) {
                auto *constraintExpr = conceptDefStmt->getConstraintExpr();
                if (constraintExpr) {
                    if (!foundC && isa<clang::RequiresExpr>(constraintExpr)) {
                        conceptCName = conceptDecl->getNameAsString();
                        foundC = true;
                    } else if (!foundD && isa<clang::TrueExpr>(constraintExpr)) {
                        conceptDName = conceptDecl->getNameAsString();
                        foundD = true;
                    }
                }
            }
        }

        if (foundC && foundD && !inserted) {
            std::string newConceptE = "template<class> concept E = " + conceptCName + "<[]<" + conceptDName + ">{}>;\n";
            std::string staticAssert = "static_assert(E<int>);\n";
            std::string insertedCode = "/*mut449*/\n" + newConceptE + staticAssert;

            SourceManager &SM = Rewrite.getSourceMgr();
            FileID FID = SM.getMainFileID();
            SourceLocation endLoc = SM.getLocForEndOfFile(FID);
            if (endLoc.isValid()) {
                Rewrite.InsertText(endLoc, insertedCode, true, true);
                inserted = true;
            }
        }
    }
}
  
void MutatorFrontendAction_449::MutatorASTConsumer_449::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateDeclConcept().bind("concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}