//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Struct_Ref_With_Lambda_344
 */ 
class MutatorFrontendAction_344 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(344)
private:
    class MutatorASTConsumer_344 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_344(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> targetVarDecls;
    };
};

//source file
#include "../include/Replace_Struct_Ref_With_Lambda_344.h"

// ========================================================================================================
#define MUT344_OUTPUT 1

void MutatorFrontendAction_344::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (!VD->getType()->isReferenceType() || !VD->getType()->getAs<clang::ReferenceType>()->getPointeeType()->isStructureType() || !VD->hasInit())
            return;

        clang::QualType type = VD->getType();
        clang::RecordType *recordType = type->getAs<clang::RecordType>();
        if (!recordType)
            return;
        clang::RecordDecl *recordDecl = recordType->getDecl();
        std::vector<std::string> fieldNames;
        for (const auto *field : recordDecl->fields()) {
            fieldNames.push_back(field->getNameAsString());
        }

        std::string declStr = "auto& [";
        for (size_t i = 0; i < fieldNames.size(); ++i) {
            declStr += fieldNames[i];
            if (i != fieldNames.size() - 1)
                declStr += ", ";
        }
        declStr += "] = " + VD->getInit()->getExprStmt()->getSourceRange().getAsRange().str() + ";";

        Rewrite.ReplaceText(VD->getSourceRange(), declStr);
        targetVarDecls.push_back(VD);
    } else if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("binaryOp")) {
        for (const auto *op : {BO->getLHS(), BO->getRHS()}) {
            if (!op->getStmtClass() == clang::Stmt::MemberExprClass)
                return;
        }

        clang::MemberExpr *lhs = BO->getLHS()->getAs<MemberExpr>();
        clang::MemberExpr *rhs = BO->getRHS()->getAs<MemberExpr>();

        clang::DeclRefExpr *lhsBase = lhs->getBase()->getAs<DeclRefExpr>();
        clang::DeclRefExpr *rhsBase = rhs->getBase()->getAs<DeclRefExpr>();

        if (!lhsBase || !rhsBase)
            return;

        clang::VarDecl *lhsVar = lhsBase->getDecl()->getAs<VarDecl>();
        clang::VarDecl *rhsVar = rhsBase->getDecl()->getAs<VarDecl>();

        if (!lhsVar || !rhsVar)
            return;

        bool lhsFound = false;
        bool rhsFound = false;
        for (const auto *vd : targetVarDecls) {
            if (lhsVar == vd)
                lhsFound = true;
            if (rhsVar == vd)
                rhsFound = true;
        }
        if (!lhsFound || !rhsFound)
            return;

        clang::QualType lhsType = lhsVar->getType();
        clang::RecordType *lhsRecordType = lhsType->getAs<clang::RecordType>();
        if (!lhsRecordType)
            return;
        clang::RecordDecl *lhsRecordDecl = lhsRecordType->getDecl();
        std::vector<std::string> lhsFieldNames;
        for (const auto *field : lhsRecordDecl->fields()) {
            lhsFieldNames.push_back(field->getNameAsString());
        }

        std::string lhsMember = lhs->getMemberName()->getNameAsString();
        std::string rhsMember = rhs->getMemberName()->getNameAsString();

        bool lhsMemberFound = false;
        bool rhsMemberFound = false;
        for (const auto &name : lhsFieldNames) {
            if (name == lhsMember)
                lhsMemberFound = true;
            if (name == rhsMember)
                rhsMemberFound = true;
        }
        if (!lhsMemberFound || !rhsMemberFound)
            return;

        std::string lambdaCapture = "[";
        for (size_t i = 0; i < lhsFieldNames.size(); ++i) {
            lambdaCapture += lhsFieldNames[i];
            if (i != lhsFieldNames.size() - 1)
                lambdaCapture += ", ";
        }
        lambdaCapture += "] { return " + lhsMember + " " + BO->getOpcodeStr() + " " + rhsMember + "; }();";

        Rewrite.ReplaceText(BO->getSourceRange(), lambdaCapture);
    }
}

void MutatorFrontendAction_344::MutatorASTConsumer_344::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher varMatcher = varDecl(
        hasType(referenceType(pointee(recordType()))),
        hasInitializer(callExpr())
    ).bind("varDecl");
    DeclarationMatcher binOpMatcher = binaryOperator(
        hasLHS(memberExpr(hasBase(declRefExpr(to(varDecl())))),
        hasRHS(memberExpr(hasBase(declRefExpr(to(varDecl()))))
    ).bind("binaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(binOpMatcher, &callback);
    matchFinder.matchAST(Context);
}