//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Zero_With_String_Cast_In_Struct_Array_466
 */ 
class MutatorFrontendAction_466 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(466)

private:
    class MutatorASTConsumer_466 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_466(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Zero_With_String_Cast_In_Struct_Array_466.h"

// ========================================================================================================
#define MUT466_OUTPUT 1

void MutatorFrontendAction_466::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        QualType arrayType = VD->getType();
        if (!arrayType->isArrayType()) return;
        const ArrayType *arrayTy = arrayType.getTypePtr();
        QualType elementType = arrayTy->getElementType();
        if (!elementType->isRecordType()) return;
        const RecordType *recordTy = elementType.getTypePtr();
        RecordDecl *recordDecl = recordTy->getDecl();

        bool hasPointerField = false;
        QualType pointerFieldType;
        for (const auto *field : recordDecl->fields()) {
            if (field->getType()->isPointerType()) {
                hasPointerField = true;
                pointerFieldType = field->getType();
                break;
            }
        }
        if (!hasPointerField) return;

        InitListExpr *initList = dyn_cast<InitListExpr>(VD->getInit());
        if (!initList) return;

        for (unsigned i = 0; i < initList->getNumInits(); ++i) {
            Expr *init = initList->getInit(i);
            if (isa<IntegerLiteral>(init) && cast<IntegerLiteral>(init)->getValue().isZero()) {
                SourceLocation zeroLoc = init->getSourceRange().getBegin();
                SourceLocation endLoc = init->getSourceRange().getEnd();

                const Type *pointerType = pointerFieldType.getTypePtr();
                const Type *pointeeType = pointerType->getAs<PointerType>()->getPointeeType().getTypePtr();
                std::string typeName = pointeeType->getCanonicalTypeInternal().getAsString();
                std::string castExpr = "(" + typeName + "*) (\"\")";

                Rewrite.ReplaceText(SourceRange(zeroLoc, endLoc), castExpr);
                break;
            }
        }
    }
}
  
void MutatorFrontendAction_466::MutatorASTConsumer_466::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(
        hasType(arrayType(hasElementType(recordType()))),
        isConstQualified(),
        hasInitializer(initListExpr())
    ).bind("arrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}