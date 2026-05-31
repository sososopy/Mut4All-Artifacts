//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_82
 */ 

class MutatorFrontendAction_82 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(82)
private:
    class MutatorASTConsumer_82 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_82(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> processedClasses;
    };
};

//source file
#include "../include/insert_virtual_base_class_82.h"

// ========================================================================================================
#define MUT82_OUTPUT 1

void MutatorFrontendAction_82::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;

        std::string className = CL->getNameAsString();
        if (processedClasses.find(className) != processedClasses.end())
            return;

        processedClasses.insert(className);

        std::string baseClassName = "VirtualBase_" + className;
        std::string baseClassDef = "class " + baseClassName + " {};";
        std::string derivedClassDef = "class " + className + " : public virtual " + baseClassName + " {};\n";

        SourceLocation insertLoc = CL->getBeginLoc();
        Rewrite.InsertTextBefore(insertLoc, baseClassDef + "\n");
        Rewrite.InsertTextAfterToken(CL->getEndLoc(), derivedClassDef);
    }
}

void MutatorFrontendAction_82::MutatorASTConsumer_82::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}