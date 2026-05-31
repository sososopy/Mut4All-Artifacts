//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Virtual_Base_Class_242
 */ 
class MutatorFrontendAction_242 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(242)

private:
    class MutatorASTConsumer_242 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_242(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> visitedClasses;
    };
};

//source file
#include "../include/add_virtual_base_class_242.h"

// ========================================================================================================
#define MUT242_OUTPUT 1

void MutatorFrontendAction_242::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if (!CL->isCompleteDefinition() || CL->isLambda())
            return;

        std::string className = CL->getNameAsString();
        if (visitedClasses.find(className) != visitedClasses.end())
            return;

        visitedClasses.insert(className);

        if (CL->getNumBases() == 0) {
            std::string virtualBase = "class VirtualBase {};\n";
            std::string newClassDef = "class " + className + " : virtual public VirtualBase ";
            auto classRange = CL->getSourceRange();
            std::string classText = stringutils::rangetoStr(*(Result.SourceManager), classRange);
            size_t pos = classText.find(className);
            if (pos != std::string::npos) {
                classText.replace(pos, className.length(), newClassDef);
                classText = "/*mut242*/" + virtualBase + classText;
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(classRange), classText);
            }
        }
    }
}

void MutatorFrontendAction_242::MutatorASTConsumer_242::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}