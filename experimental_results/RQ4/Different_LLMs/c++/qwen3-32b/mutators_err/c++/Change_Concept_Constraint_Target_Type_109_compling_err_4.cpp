//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/ExprCXX.h"

/**
 * Change_Concept_Constraint_Target_Type_109
 */ 
class MutatorFrontendAction_109 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(109)

private:
    class MutatorASTConsumer_109 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_109(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<SourceLocation> targetLocations;
    };
};

//source file
#include "../include/change_concept_constraint_target_type_109.h"

// ========================================================================================================
#define MUT109_OUTPUT 1

void MutatorFrontendAction_109::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RE = Result.Nodes.getNodeAs<clang::RequiresExpr>("requirement")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(RE->getLocation()))
            return;

        for (unsigned i = 0; i < RE->getNumRequirements(); ++i) {
            if (auto *TR = dyn_cast<clang::TypeRequirement>(RE->getRequirement(i))) {
                if (auto *CD = TR->getConcept()) {
                    if (CD->getName() == "convertible_to") {
                        auto args = TR->getTemplateArgs();
                        if (args.size() >= 1) {
                            auto typeLoc = args[0].getSourceRange();
                            std::string newType = Random::getRandomPrimitiveType(); // int, bool, double, etc.
                            Rewrite.ReplaceText(typeLoc, newType);
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_109::MutatorASTConsumer_109::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::requiresExpr().bind("requirement");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}