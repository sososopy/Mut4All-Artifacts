//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * modify_anonymous_struct_static_assert_43
 */ 
class MutatorFrontendAction_43 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(43)

private:
    class MutatorASTConsumer_43 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_43(Rewriter &R) : TheRewriter(R) {}
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
        const TypeDecl *existingTypeDecl = nullptr;
    };
};

//source file
#include "../include/modify_anonymous_struct_static_assert_43.h"

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *anonStruct = Result.Nodes.getNodeAs<clang::RecordDecl>("AnonStruct")) {
        // Filter nodes in header files
        if (!anonStruct || !Result.Context->getSourceManager().isWrittenInMainFile(
                anonStruct->getLocation()))
            return;

        // Find an existing type in the program to use in the static assertion
        if (!existingTypeDecl) {
            if (auto *typeNode = Result.Nodes.getNodeAs<clang::TypeDecl>("ExistingType")) {
                existingTypeDecl = typeNode;
            }
        }

        if (existingTypeDecl) {
            // Get the source code text of target node
            std::string structName = "NamedStruct_" + std::to_string(rand() % 1000);
            std::string existingTypeName = existingTypeDecl->getNameAsString();
            std::string newStaticAssert = "_Static_assert(sizeof(" + existingTypeName + ") == sizeof(int), \"Size mismatch\");";

            // Perform mutation on the source code text by applying string replacement
            std::string newStruct = "struct " + structName + " { " + newStaticAssert + " };";
            Rewrite.ReplaceText(anonStruct->getSourceRange(), newStruct);
        }
    }
}

void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    auto anonStructMatcher = recordDecl(isAnonymousStructOrUnion()).bind("AnonStruct");
    auto existingTypeMatcher = typeDecl().bind("ExistingType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(anonStructMatcher, &callback);
    matchFinder.addMatcher(existingTypeMatcher, &callback);
    matchFinder.matchAST(Context);
}