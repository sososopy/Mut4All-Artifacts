//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Initializer_With_Function_Call_184
 */ 
class MutatorFrontendAction_184 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(184)

private:
    class MutatorASTConsumer_184 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_184(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Member_Initializer_With_Function_Call_184.h"

// ========================================================================================================
#define MUT184_OUTPUT 1

void MutatorFrontendAction_184::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *classDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!classDecl || !Result.Context->getSourceManager().isWrittenInMainFile(classDecl->getLocation()))
            return;

        if ((!classDecl->isStruct() && !classDecl->isClass()) || classDecl->isLambda())
            return;

        if (!classDecl->isCompleteDefinition())
            return;

        std::string className = classDecl->getNameAsString();
        if (className.empty())
            return;

        const clang::FieldDecl *targetField = nullptr;
        for (const auto *field : classDecl->fields()) {
            if (field->hasInClassInitializer()) {
                QualType fieldType = field->getType();
                if (fieldType->isIntegerType()) {
                    targetField = field;
                    break;
                }
            }
        }

        if (!targetField)
            return;

        std::string forwardDecl = "struct " + className + ";";
        std::string funcDecl = "int g(const " + className + "&);";

        SourceLocation classLoc = classDecl->getBeginLoc();
        Rewrite.InsertTextBefore(classLoc, forwardDecl + "\n" + funcDecl + "\n");

        const clang::Expr *initExpr = targetField->getInClassInitializer();
        SourceRange initRange = initExpr->getSourceRange();
        std::string newInit = "g(" + className + "())";
        Rewrite.ReplaceText(initRange, newInit);

        bool hasConstructor = false;
        for (const auto *ctor : classDecl->ctors()) {
            if (!ctor->isImplicit())
                hasConstructor = true;
        }

        if (!hasConstructor) {
            std::string constructor = className + "() { }";
            SourceLocation endLoc = classDecl->getEndLoc();
            Rewrite.InsertText(endLoc, "\n    " + constructor + ";", true, true);
        }
    }
}
  
void MutatorFrontendAction_184::MutatorASTConsumer_184::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(
        has(fieldDecl(
            hasInitializer(), 
            hasType(isInteger())
        )
    )).bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}