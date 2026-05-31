//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/Type.h"

/**
 * Remove_Semicolon_And_Typo_In_Template_Struct_181
 */ 

class MutatorFrontendAction_181 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(181)

private:
    class MutatorASTConsumer_181 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_181(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Semicolon_And_Typo_In_Template_Struct_181.h"

// ========================================================================================================
#define MUT181_OUTPUT 1

void MutatorFrontendAction_181::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *OuterStruct = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("outerStruct")) {
        if (!OuterStruct || !Result.Context->getSourceManager().isWrittenInMainFile(
                       OuterStruct->getLocation()))
            return;

        std::vector<const clang::CXXRecordDecl *> nestedStructs;
        for (const auto *Decl : OuterStruct->decls()) {
            if (const auto *Nested = clang::dyn_cast<clang::CXXRecordDecl>(Decl)) {
                nestedStructs.push_back(Nested);
            }
        }

        for (size_t i = 0; i < nestedStructs.size(); ++i) {
            const clang::CXXRecordDecl *B = nestedStructs[i];
            for (size_t j = 0; j < nestedStructs.size(); ++j) {
                const clang::CXXRecordDecl *C = nestedStructs[j];
                if (i == j) continue;
                for (const auto &Base : C->bases()) {
                    if (Base.getType()->getAs<clang::RecordType>()->getDecl() == B) {
                        // Remove semicolon after B
                        SourceLocation BEndLoc = B->getEndLoc();
                        if (BEndLoc.isValid()) {
                            Rewrite.ReplaceText(BEndLoc, 1, "");
                        }

                        // Replace B with Btypo in C's base
                        for (const auto &Base : C->bases()) {
                            clang::TypeLoc BaseTypeLoc = Base.getType().getTypeLoc();
                            SourceLocation StartLoc = BaseTypeLoc.getBeginLoc();
                            SourceLocation EndLoc = BaseTypeLoc.getEndLoc();
                            if (BaseTypeLoc.getType()->getAs<clang::RecordType>()->getDecl() == B) {
                                Rewrite.ReplaceText(clang::SourceRange(StartLoc, EndLoc), "Btypo");
                            }
                        }
                        return; // Apply to first found pair
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_181::MutatorASTConsumer_181::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::cxxRecordDecl(clang::isTemplateInstantiated()).bind("outerStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}