//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * modify_anonymous_struct_with_static_assertions_43
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
    };
};

//source file
#include "../include/modify_anonymous_struct_with_static_assertions_43.h"
#include <type_traits>

#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::RecordDecl>("AnonymousStruct")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        
        auto staticAssertDecl = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert");
        if (!staticAssertDecl)
            return;

        std::string newStaticAssert = "static_assert(!std::is_same<decltype(*ptr), int>::value, \"Pointer type is unexpectedly int\");";
        std::string memberVariable = "int member;";

        std::string structContent = "{\n" + newStaticAssert + "\n" + memberVariable + "\n};";
        Rewrite.ReplaceText(MT->getSourceRange(), structContent);
    }
}
  
void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(isAnonymousStructOrUnion(), hasDescendant(staticAssertDecl())).bind("AnonymousStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}