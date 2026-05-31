//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Explicit_Object_Parameter_405
 */ 
class MutatorFrontendAction_405 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(405)

private:
    class MutatorASTConsumer_405 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_405(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Add_Explicit_Object_Parameter_405.h"

// ========================================================================================================
#define MUT405_OUTPUT 1

void MutatorFrontendAction_405::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        if (MT->isStatic())
            return;
        QualType thisType = MT->getThisType();
        const clang::PointerType *ptrType = thisType->getAs<clang::PointerType>();
        if (!ptrType)
            return;
        QualType pointeeType = ptrType->getPointeeType();
        QualType refType = Result.Context->getLValueReferenceType(pointeeType);
        std::string typeStr;
        refType.getAsStringInternal(typeStr, Result.Context->getPrintingPolicy());
        TypeLoc typeLoc = MT->getTypeSourceInfo()->getTypeLoc();
        if (auto funcTypeLoc = typeLoc.getAs<clang::FunctionProtoTypeLoc>()) {
            SourceLocation lParenLoc = funcTypeLoc.getLParenLoc();
            if (lParenLoc.isValid()) {
                std::string paramDecl = "this " + typeStr + " x";
                if (MT->param_size() > 0)
                    paramDecl += ", ";
                Rewrite.InsertTextAfter(lParenLoc, paramDecl);
            }
        }
    }
}
  
void MutatorFrontendAction_405::MutatorASTConsumer_405::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(unless(clang::ast_matchers::isStatic())).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}