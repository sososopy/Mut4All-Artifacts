//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Template_Union_With_Nontrivial_Class_255
 */ 
class MutatorFrontendAction_255 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(255)

private:
    class MutatorASTConsumer_255 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_255(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> nonTrivialClasses;
    };

};

//source file
#include "../include/Introduce_Template_Union_With_Nontrivial_Class_255.h"

// ========================================================================================================
#define MUT255_OUTPUT 1

void MutatorFrontendAction_255::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *nonTrivial = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("nontrivial")) {
        if (!nonTrivial || !Result.Context->getSourceManager().isWrittenInMainFile(nonTrivial->getLocation()))
            return;
        nonTrivialClasses.push_back(nonTrivial);
    } else if (auto *targetClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("targetClass")) {
        if (!targetClass || !Result.Context->getSourceManager().isWrittenInMainFile(targetClass->getLocation()))
            return;
        if (nonTrivialClasses.empty())
            return;
        const clang::CXXRecordDecl* selectedNonTrivial = nonTrivialClasses[getrandom::getRandomIndex(nonTrivialClasses.size())];
        auto sourceManager = Result.Context->getSourceManager();
        auto targetRange = targetClass->getSourceRange();
        if (sourceManager->isInMainFile(targetRange.getBegin()) && sourceManager->isInMainFile(targetRange.getEnd())) {
            std::string originalCode = stringutils::rangetoStr(*sourceManager, targetRange);
            size_t openBracePos = originalCode.find('{');
            if (openBracePos == std::string::npos)
                return;
            std::string partBeforeBrace = originalCode.substr(0, openBracePos + 1);
            std::string partAfterBrace = originalCode.substr(openBracePos + 1);
            std::string unionCode = "\n    union {\n        " + selectedNonTrivial->getNameAsString() + "<T> a;\n    };\n";
            std::string modifiedCode = "template<typename T>\n" + partBeforeBrace + unionCode + partAfterBrace;
            Rewrite.ReplaceText(targetRange, modifiedCode);
        }
    }
}
  
void MutatorFrontendAction_255::MutatorASTConsumer_255::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher nonTrivialMatcher = cxxRecordDecl(
        anyOf(
            hasAnyConstructor(cxxConstructorDecl(isUserProvided())),
            hasDestructor(cxxDestructorDecl(isUserProvided()))
        )
    ).bind("nontrivial");
    DeclarationMatcher targetMatcher = cxxRecordDecl(
        unless(isTemplateInstantiation()),
        unless(isLambda()),
        isCompleteDefinition()
    ).bind("targetClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(nonTrivialMatcher, &callback);
    matchFinder.addMatcher(targetMatcher, &callback);
    matchFinder.matchAST(Context);
}