//header file
#pragma once
#include "Mutator_base.h"

/**
 * Export_Nested_Struct_Without_Definition_290
 */ 
class MutatorFrontendAction_290 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(290)

private:
    class MutatorASTConsumer_290 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_290(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Export_Nested_Struct_Without_Definition_290.h"

// ========================================================================================================
#define MUT290_OUTPUT 1

void MutatorFrontendAction_290::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Outer = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("OuterStruct")) {
        if (!Outer || !Result.Context->getSourceManager().isWrittenInMainFile(Outer->getLocation()))
            return;

        for (auto *D : Outer->decls()) {
            if (auto *Inner = llvm::dyn_cast<clang::RecordDecl>(D)) {
                if (Inner->isCompleteDefinition())
                    continue;

                // Get the source code text of target node
                std::string outerName = Outer->getNameAsString();
                std::string innerName = Inner->getNameAsString();
                std::string exportStmt = "\nexport struct " + outerName + "::" + innerName + " {};";

                // Perform mutation on the source code text by applying string replacement
                Rewrite.InsertTextAfterToken(Outer->getEndLoc(), exportStmt);
            }
        }
    }
}

void MutatorFrontendAction_290::MutatorASTConsumer_290::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(hasDescendant(recordDecl(unless(isDefinition())).bind("InnerStruct"))).bind("OuterStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}