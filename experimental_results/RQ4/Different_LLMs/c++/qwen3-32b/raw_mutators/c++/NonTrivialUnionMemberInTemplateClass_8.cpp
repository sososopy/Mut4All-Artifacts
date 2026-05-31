//header file
#pragma once
#include "Mutator_base.h"

/**
 * NonTrivialUnionMemberInTemplateClass_8
 */ 
class MutatorFrontendAction_8 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(8)
private:
    class MutatorASTConsumer_8 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_8(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/NonTrivialUnionMemberInTemplateClass_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("classTemplate")) {
        // Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        // Check if NonTrivial exists
        bool hasNonTrivial = false;
        for (const auto &Decl : Result.Context->getTranslationUnitDecl()->decls()) {
            if (const auto *RD = dyn_cast<CXXRecordDecl>(Decl)) {
                if (RD->getNameAsString() == "NonTrivial") {
                    hasNonTrivial = true;
                    break;
                }
            }
        }

        if (!hasNonTrivial) {
            // Insert NonTrivial before the class template
            SourceLocation insertLoc = MT->getLocation();
            std::string nonTrivialCode = "struct NonTrivial { NonTrivial() {} };\n";
            Rewrite.InsertTextBefore(insertLoc, nonTrivialCode);
        }

        // Replace the trivial field in the union
        if (auto *FD = Result.Nodes.getNodeAs<FieldDecl>("trivialField")) {
            SourceRange typeRange = FD->getTypeSourceInfo()->getTypeLoc().getSourceRange();
            Rewrite.ReplaceText(typeRange, "NonTrivial");
        }
    }
}

void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(
        has(cxxRecordDecl(
            has(unionDecl(
                has(fieldDecl(hasType(isBuiltin())).bind("trivialField")))))).bind("classTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}