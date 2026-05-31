//header file
#pragma once
#include "Mutator_base.h"

/**
 * Redefine_Struct_With_Forward_Declaration_287
 */ 
class MutatorFrontendAction_287 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(287)

private:
    class MutatorASTConsumer_287 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_287(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Redefine_Struct_With_Forward_Declaration_287.h"

// ========================================================================================================
#define MUT287_OUTPUT 1

void MutatorFrontendAction_287::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        if (!MT->isStruct() || MT->isLambda())
            return;
        std::string structName = MT->getNameAsString();
        if (structName.empty())
            return;
        SourceRange braceRange = MT->getBraceRange();
        if (braceRange.isInvalid())
            return;
        SourceLocation openBraceLoc = braceRange.getBegin();
        SourceLocation closeBraceLoc = braceRange.getEnd();
        std::string forwardDecl = "struct " + structName + ";";
        Rewrite.InsertText(openBraceLoc, forwardDecl + "\n", true, true);
        std::string newStructCode = "struct " + structName + " { float b; };";
        Rewrite.InsertText(closeBraceLoc, "\n" + newStructCode, true, true);
    }
}

void MutatorFrontendAction_287::MutatorASTConsumer_287::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}