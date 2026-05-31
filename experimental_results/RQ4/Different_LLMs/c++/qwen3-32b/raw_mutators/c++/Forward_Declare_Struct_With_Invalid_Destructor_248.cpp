//header file
#pragma once
#include "Mutator_base.h"

/**
 * Forward_Declare_Struct_With_Invalid_Destructor_248
 */ 
class MutatorFrontendAction_248 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(248)

private:
    class MutatorASTConsumer_248 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_248(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Forward_Declare_Struct_With_Invalid_Destructor_248.h"

// ========================================================================================================
#define MUT248_OUTPUT 1

void MutatorFrontendAction_248::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("structDecl")) {
        // Filter nodes in header files
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        // Check if it's a struct or class and has a body
        if (!(RD->isStruct() || RD->isClass()) || !RD->isCompleteDefinition())
            return;
        // Get the name of the struct
        std::string structName = RD->getNameAsString();
        if (structName.empty())
            return;
        // Get the source range of the struct
        SourceLocation start = RD->getBeginLoc();
        SourceLocation end = RD->getEndLoc();
        if (start.isInvalid() || end.isInvalid())
            return;
        // Replace the struct definition with forward declaration
        std::string forwardDecl = "struct " + structName + ";";
        Rewrite.ReplaceText(SourceRange(start, end), forwardDecl);
        // Insert the invalid destructor declaration right after
        std::string dtorDecl = structName + "::~" + structName + ";";
        Rewrite.InsertTextAfterToken(end, "\n" + dtorDecl + " /*mut248*/");
    }
}

void MutatorFrontendAction_248::MutatorASTConsumer_248::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("structDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}