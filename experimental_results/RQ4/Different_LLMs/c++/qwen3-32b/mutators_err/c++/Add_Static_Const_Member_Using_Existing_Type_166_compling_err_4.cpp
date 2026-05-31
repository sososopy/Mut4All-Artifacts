//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Static_Const_Member_Using_Existing_Type_166
 */ 
class MutatorFrontendAction_166 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(166)

private:
    class MutatorASTConsumer_166 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_166(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Static_Const_Member_Using_Existing_Type_166.h"

// ========================================================================================================
#define MUT166_OUTPUT 1

void MutatorFrontendAction_166::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classTemplate")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        
        if (!CL->isClass() || CL->getTemplateSpecializationKind() == clang::TSK_Undeclared)
            return;
        
        bool hasStaticConst = false;
        for (const auto *FD : CL->fields()) {
            if (FD->isStatic() && FD->getType().isConstQualified()) {
                hasStaticConst = true;
                break;
            }
        }
        if (hasStaticConst)
            return;
        
        std::string typeName = "int";
        for (const auto *FD : CL->fields()) {
            if (!FD->isStatic()) {
                typeName = FD->getType().getCanonicalType().getAsString();
                break;
            }
        }
        
        std::string decl = "    static const " + typeName + " mut_166;\n";
        decl = "/*mut166*/" + decl;
        
        SourceLocation endLoc = CL->getEndLoc();
        if (endLoc.isValid()) {
            Rewrite.InsertTextBefore(endLoc, decl);
        }
    }
}

void MutatorFrontendAction_166::MutatorASTConsumer_166::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(
        isClass(),
        isTemplateInstantiation(),
        unless(hasDescendant(fieldDecl(isStatic(), hasType(isConstQualified()))))
    ).bind("classTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}