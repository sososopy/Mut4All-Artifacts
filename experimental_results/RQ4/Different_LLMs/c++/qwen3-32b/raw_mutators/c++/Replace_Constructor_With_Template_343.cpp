//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constructor_With_Template_343
 */ 
class MutatorFrontendAction_343 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(343)

private:
    class MutatorASTConsumer_343 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_343(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> insertedTemplates;
        int templateCounter = 0;
    };
};

//source file
#include "../include/Replace_Constructor_With_Template_343.h"

// ========================================================================================================
#define MUT343_OUTPUT 1

void MutatorFrontendAction_343::Callback::run(const MatchFinder::MatchResult &Result) {
    SourceManager &SM = Rewrite.getSourceMgr();
    if (auto *CE = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("Constructor")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        ASTContext *Context = Result.Context;
        QualType argType = CE->getArg(0)->getType();
        std::string argTypeName = Context->getPrintingPolicy().ConvertType(argType);

        std::string templateName = "X_" + std::to_string(templateCounter++);
        std::string templateDecl = "template<typename T, typename U> struct " + templateName + " { " + templateName + "(T(), U()) = delete; };\n";

        if (insertedTemplates.find(templateName) == insertedTemplates.end()) {
            SourceLocation fileStart = SM.getLocForStartOfFile(SM.getMainFileID());
            Rewrite.InsertText(fileStart, templateDecl);
            insertedTemplates.insert(templateName);
        }

        SourceRange argRange = CE->getArg(0)->getSourceRange();
        std::string argStr = stringutils::rangetoStr(*Result.SourceManager, argRange);

        std::string replacement = templateName + "<" + argTypeName + ", int>(" + argStr + ", int{})";
        Rewrite.ReplaceText(CE->getSourceRange(), replacement);
    }
}

void MutatorFrontendAction_343::MutatorASTConsumer_343::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxConstructExpr().bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}