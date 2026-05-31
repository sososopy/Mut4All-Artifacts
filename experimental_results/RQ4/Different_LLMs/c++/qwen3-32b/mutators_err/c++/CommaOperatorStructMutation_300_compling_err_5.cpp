#pragma once
#include "Mutator_base.h"

/**
 * CommaOperatorStructMutation_300
 */ 
class MutatorFrontendAction_300 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(300)

private:
    class MutatorASTConsumer_300 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_300(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::VarDecl *> nonStructVars;
    };
};

//source file
#include "../include/CommaOperatorStructMutation_300.h"

// ========================================================================================================
#define MUT300_OUTPUT 1

void MutatorFrontendAction_300::Callback::run(const MatchFinder::MatchResult &Result) {
    // Collect non-struct variables
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("nonStructVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        QualType type = VD->getType();
        if (!type->isStructureType() && !type->isUnionType()) {
            nonStructVars.push_back(VD);
        }
        return;
    }

    // Handle member access expressions
    if (auto *ME = Result.Nodes.getNodeAs<clang::MemberExpr>("memberExpr")) {
        if (!ME || !Result.Context->getSourceManager().isWrittenInMainFile(ME->getExprLoc()))
            return;
        if (!nonStructVars.empty()) {
            QualType baseType = ME->getBase()->getType();
            if (baseType->isStructureType() || baseType->isUnionType()) {
                SourceManager &SM = *Result.SourceManager;
                std::string baseText = stringutils::rangetoStr(SM, ME->getBase()->getSourceRange());
                std::string memberText = ME->getMemberDecl()->getName().str();
                std::string varName = nonStructVars[0]->getNameAsString();
                std::string mutatedText = "(((" + baseText + ", " + varName + "), " + baseText + ")." + memberText + ")";
                Rewrite.ReplaceText(ME->getSourceRange(), mutatedText);
            }
        }
    }

    // Handle assignment expressions
    if (auto *BE = Result.Nodes.getNodeAs<clang::BinaryOperator>("assignmentExpr")) {
        if (!BE || !Result.Context->getSourceManager().isWrittenInMainFile(BE->getBeginLoc()))
            return;
        if (BE->getOpcode() == BO_Assign) {
            if (!nonStructVars.empty()) {
                QualType lhsType = BE->getLHS()->getType();
                if (lhsType->isStructureType() || lhsType->isUnionType()) {
                    SourceManager &SM = *Result.SourceManager;
                    std::string lhsText = stringutils::rangetoStr(SM, BE->getLHS()->getSourceRange());
                    std::string varName = nonStructVars[0]->getNameAsString();
                    std::string mutatedText = "(((" + lhsText + " = " + varName + "), " + lhsText + ").data";
                    Rewrite.ReplaceText(BE->getSourceRange(), mutatedText);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_300::MutatorASTConsumer_300::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher varMatcher = varDecl().bind("nonStructVar");
    DeclarationMatcher memberMatcher = memberExpr(hasType(anyOf(isUnionType(), isStructureType()))).bind("memberExpr");
    DeclarationMatcher assignmentMatcher = binaryOperator(hasOperatorName("="), hasLHS(declRefExpr(to(varDecl(hasType(anyOf(isUnionType(), isStructureType()))))))).bind("assignmentExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(memberMatcher, &callback);
    matchFinder.addMatcher(assignmentMatcher, &callback);
    matchFinder.matchAST(Context);
}