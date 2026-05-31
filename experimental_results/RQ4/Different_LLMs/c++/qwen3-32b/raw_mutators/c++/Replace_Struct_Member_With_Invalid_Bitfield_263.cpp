//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Struct_Member_With_Invalid_Bitfield_263
 */ 
class MutatorFrontendAction_263 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(263)

private:
    class MutatorASTConsumer_263 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_263(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> invalidTypeNames;
    };
};

//source file
#include "../include/Replace_Struct_Member_With_Invalid_Bitfield_263.h"

// ========================================================================================================
#define MUT263_OUTPUT 1

void MutatorFrontendAction_263::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("field")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!FD->getParent()->isStruct() && !FD->getParent()->isUnion())
            return;

        if (invalidTypeNames.empty())
            return;

        std::vector<std::string> tempVector(invalidTypeNames.begin(), invalidTypeNames.end());
        std::string typeName = getrandom::getRandomElement(tempVector);

        std::string replacement = "struct {\n    " + typeName + " : 8;\n};";
        Rewrite.ReplaceText(FD->getSourceRange(), replacement);
    } else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;

        invalidTypeNames.insert(RD->getNameAsString());
    }
}

void MutatorFrontendAction_263::MutatorASTConsumer_263::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher fieldMatcher = fieldDecl(isInCXXRecordDecl()).bind("field");
    DeclarationMatcher recordMatcher = cxxRecordDecl().bind("record");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.matchAST(Context);
}