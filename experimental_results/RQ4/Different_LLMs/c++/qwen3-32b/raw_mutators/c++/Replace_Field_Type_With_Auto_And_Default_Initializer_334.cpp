//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Field_Type_With_Auto_And_Default_Initializer_334
 */ 
class MutatorFrontendAction_Replace_Field_Type_With_Auto_And_Default_Initializer_334 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Replace_Field_Type_With_Auto_And_Default_Initializer_334)

private:
    class MutatorASTConsumer_Replace_Field_Type_With_Auto_And_Default_Initializer_334 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Field_Type_With_Auto_And_Default_Initializer_334(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Field_Type_With_Auto_And_Default_Initializer_334.h"

// ========================================================================================================
#define MUT_Replace_Field_Type_With_Auto_And_Default_Initializer_334_OUTPUT 1

void MutatorFrontendAction_Replace_Field_Type_With_Auto_And_Default_Initializer_334::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("fieldDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        QualType fieldType = FD->getType();
        std::string initializer;

        if (fieldType->isIntegralOrEnumerationType()) {
            initializer = "0";
        } else if (fieldType->isPointerType()) {
            initializer = "nullptr";
        } else if (fieldType->isFloatingPointType()) {
            initializer = "0.0";
        } else if (fieldType->isStructureOrClassType()) {
            initializer = "{}";
        } else if (fieldType->isArrayType()) {
            initializer = "{}";
        } else if (fieldType->isBuiltinType()) {
            if (fieldType->isArithmeticType()) {
                initializer = "0";
            } else {
                initializer = "nullptr";
            }
        } else {
            initializer = "{}";
        }

        if (initializer.empty()) return;

        std::string newDecl = "auto " + FD->getNameAsString() + " = " + initializer + ";";
        Rewrite.ReplaceText(FD->getSourceRange(), newDecl);
    }
}

void MutatorFrontendAction_Replace_Field_Type_With_Auto_And_Default_Initializer_334::MutatorASTConsumer_Replace_Field_Type_With_Auto_And_Default_Initializer_334::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(unless(isStatic()), unless(hasInClassInitializer()), unless(isReferenceType())).bind("fieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}