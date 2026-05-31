//header file
#pragma once
#include "Mutator_base.h"

/**
 * Create_Circular_Concept_Function_Dependency_68
 */ 
class MutatorFrontendAction_68 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(68)
private:
    class MutatorASTConsumer_68 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_68(Rewriter &R) : TheRewriter(R) {}
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
        clang::FunctionDecl *TargetFunction = nullptr;
        std::string ConceptName;
    };
};

//source file
#include "../include/Create_Circular_Concept_Function_Dependency_68.h"

// ========================================================================================================
#define MUT68_OUTPUT 1

void MutatorFrontendAction_68::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concept")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        // Extract concept name
        ConceptName = CD->getNameAsString();
        // Extract target function from requires clause
        if (auto *RE = CD->getRequiresClause()) {
            for (const auto *CE : RE->getRequirements()) {
                if (const auto *CR = dyn_cast<clang::ConstraintLogicalAnd>(CE)) {
                    for (const auto *R : CR->getConstraints()) {
                        if (const auto *RE = dyn_cast<clang::RequiresExpr>(R)) {
                            for (const auto *E : RE->getRequirements()) {
                                if (const auto *Call = dyn_cast<clang::CallExpr>(E)) {
                                    if (auto *FD = Call->getCalleeDecl()) {
                                        if (FD && FD->isFunctionOrFunctionTemplate()) {
                                            TargetFunction = FD;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        // Check if this is the target function
        if (FD == TargetFunction) {
            // Modify parameters to use concept constraint
            for (const auto *Param : FD->parameters()) {
                SourceLocation StartLoc = Param->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
                SourceLocation EndLoc = Param->getTypeSourceInfo()->getTypeLoc().getEndLoc();
                if (StartLoc.isValid() && EndLoc.isValid()) {
                    Rewrite.ReplaceText(SourceRange(StartLoc, EndLoc), ConceptName + " auto");
                }
            }
        }
    }
}
  
void MutatorFrontendAction_68::MutatorASTConsumer_68::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Match concepts with requires clause referencing a function
    DeclarationMatcher conceptMatcher = 
        conceptDecl(
            hasRequiresClause(
                requiresExpr(
                    has(
                        callExpr(
                            callee(functionDecl().bind("func"))
                        )
                    )
                )
            )
        ).bind("Concept");
    // Match functions that may be modified
    DeclarationMatcher functionMatcher = 
        functionDecl(
            unless(hasAttr(attr::Concept)),
            unless(hasAttr(attr::ConceptRequirement))
        ).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}