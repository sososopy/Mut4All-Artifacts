//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_446
 */ 
class MutatorFrontendAction_446 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(446)

private:
    class MutatorASTConsumer_446 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_446(Rewriter &R) : TheRewriter(R) {}
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
        std::unordered_set<const clang::CXXRecordDecl *> processedClasses;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_446.h"

// ========================================================================================================
#define MUT446_OUTPUT 1

void MutatorFrontendAction_446::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RecursiveClass")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        
        if (processedClasses.find(RD) != processedClasses.end())
            return;

        processedClasses.insert(RD);

        if (!RD->isCompleteDefinition())
            return;

        std::string className = RD->getNameAsString();
        std::string templateDecl = "template<int N> struct " + className + " {\n"
                                   "  " + className + "<N-1> member;\n"
                                   "};\n"
                                   "template<> struct " + className + "<0> {};\n";
        
        std::string insertionText = "/*mut446*/" + templateDecl;
        Rewrite.InsertTextBefore(RD->getBeginLoc(), insertionText);
    }
}

void MutatorFrontendAction_446::MutatorASTConsumer_446::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition(), unless(isLambda())).bind("RecursiveClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}