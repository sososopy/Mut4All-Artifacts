//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Requires_Clause_Operator_74
 */ 
class MutatorFrontendAction_74 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(74)

private:
    class MutatorASTConsumer_74 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_74(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Alter_Requires_Clause_Operator_74.h"

// ========================================================================================================
#define MUT74_OUTPUT 1

void MutatorFrontendAction_74::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RE = Result.Nodes.getNodeAs<clang::RequiresExpr>("require")) {
        if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RE->getLocation()))
            return;
        Expr *constraint = RE->getConstraintExpr();
        if (auto *BO = dyn_cast<BinaryOperator>(constraint)) {
            if (BO->isComparisonOp()) {
                BinaryOperator::Opcode oldOp = BO->getOpcode();
                BinaryOperator::Opcode newOp;
                switch (oldOp) {
                    case BO_GT: newOp = BO_LT; break;
                    case BO_LT: newOp = BO_GT; break;
                    case BO_GE: newOp = BO_LE; break;
                    case BO_LE: newOp = BO_GE; break;
                    case BO_EQ: newOp = BO_NE; break;
                    case BO_NE: newOp = BO_EQ; break;
                    default: return;
                }

                SourceLocation opLoc = BO->getOperatorLoc();
                std::string newOpStr;
                switch (newOp) {
                    case BO_LT: newOpStr = "<"; break;
                    case BO_GT: newOpStr = ">"; break;
                    case BO_LE: newOpStr = "<="; break;
                    case BO_GE: newOpStr = ">="; break;
                    case BO_EQ: newOpStr = "=="; break;
                    case BO_NE: newOpStr = "!="; break;
                    default: return;
                }

                size_t originalLength = 1;
                switch (oldOp) {
                    case BO_EQ:
                    case BO_NE:
                    case BO_LE:
                    case BO_GE:
                        originalLength = 2;
                        break;
                    default:
                        originalLength = 1;
                }

                Rewrite.ReplaceText(opLoc, originalLength, newOpStr);
            }
        }
    }
}
  
void MutatorFrontendAction_74::MutatorASTConsumer_74::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = requiresExpr().bind("require");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}