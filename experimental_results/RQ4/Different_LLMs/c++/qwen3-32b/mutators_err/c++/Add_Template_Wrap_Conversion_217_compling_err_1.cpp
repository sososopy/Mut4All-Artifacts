//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Template_Wrap_Conversion_217
 */ 
class MutatorFrontendAction_217 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(217)

private:
    class MutatorASTConsumer_217 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_217(Rewriter &R) : TheRewriter(R) {}
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
        bool wrapFound = false;
        std::string mutatedClassName;
    };
};

//source file
#include "../include/Add_Template_Wrap_Conversion_217.h"

// ========================================================================================================
#define MUT217_OUTPUT 1

void MutatorFrontendAction_217::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *wrapDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("wrapDecl")) {
        wrapFound = true;
    } else if (auto *record = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classDecl")) {
        if (!record || !Result.Context->getSourceManager().isWrittenInMainFile(record->getLocation()))
            return;
        std::string className = record->getNameAsString();
        mutatedClassName = className;
        std::string convOp = "constexpr operator Wrap<" + className + ">(this const " + className + "& self) { return Wrap<" + className + ">{self}; }\n";
        std::string opShift = "constexpr int operator<< (this Wrap<" + className + "> self, int i) { return self.base + i; }\n";
        Rewrite.InsertTextBefore(record->getEndLoc(), convOp);
        Rewrite.InsertTextBefore(record->getEndLoc(), opShift);
    } else if (auto *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (!func || !Result.Context->getSourceManager().isWrittenInMainFile(func->getLocation()))
            return;
        if (mutatedClassName.empty())
            return;
        std::string code = "constexpr " + mutatedClassName + " s;\nstatic_assert((s << 11) == undefined_var);\n";
        Rewrite.InsertTextAfterToken(func->getBody()->getBeginLoc(), code);
    }
}
  
void MutatorFrontendAction_217::MutatorASTConsumer_217::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher wrapMatcher = cxxRecordDecl(hasName("Wrap")).bind("wrapDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(wrapMatcher, &callback);
    matchFinder.matchAST(Context);
    
    if (!callback.wrapFound) {
        SourceManager &SM = TheRewriter.getSourceMgr();
        FileID FID = SM.getMainFileID();
        SourceLocation Loc = SM.getLocForStartOfFile(FID);
        TheRewriter.InsertText(Loc, "template <typename T>\nstruct Wrap : T {};\n", true, true);
    }
    
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("classDecl");
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
    
    DeclarationMatcher funcMatcher = functionDecl(hasBody(compoundStmt())).bind("funcDecl");
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}