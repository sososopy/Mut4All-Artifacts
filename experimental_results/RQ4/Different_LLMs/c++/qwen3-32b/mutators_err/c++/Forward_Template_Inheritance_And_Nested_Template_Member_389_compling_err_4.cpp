//header file
#pragma once
#include "Mutator_base.h"

/**
 * Forward_Template_Inheritance_And_Nested_Template_Member_389
 */ 
class MutatorFrontendAction_389 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(389)

private:
    class MutatorASTConsumer_389 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_389(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        bool forwardTemplateDeclared = false;
        bool nestedTemplateDeclared = false;
        std::vector<const clang::CXXRecordDecl *> templateClasses;
    };
};

//source file
#include "../include/Forward_Template_Inheritance_And_Nested_Template_Member_389.h"

// ========================================================================================================
#define MUT389_OUTPUT 1

void MutatorFrontendAction_389::Callback::run(const MatchFinder::MatchResult &Result) {
    SourceManager &SM = Rewrite.getSourceMgr();
    //Handle ForwardTemplateInheritance
    if (auto *classDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classNoBase")) {
        if (!classDecl || !Result.Context->getSourceManager().isWrittenInMainFile(classDecl->getLocation()))
            return;
        if (classDecl->isLambda() || !classDecl->isCompleteDefinition())
            return;
        if (classDecl->getNumBases() == 0) {
            if (!forwardTemplateDeclared) {
                SourceLocation startLoc = SM.getLocForStartOfFile(SM.getMainFileID());
                Rewrite.InsertText(startLoc, "template <int> class ForwardTemplate;\n");
                forwardTemplateDeclared = true;
            }
            auto originalText = stringutils::rangetoStr(*Result.SourceManager, classDecl->getSourceRange());
            size_t openBracePos = originalText.find("{");
            if (openBracePos != std::string::npos) {
                originalText.insert(openBracePos, " : ForwardTemplate<1>");
                Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(classDecl->getSourceRange()), originalText);
            }
        }
    }
    //Handle NestedTemplateMember
    else if (auto *templateDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("templateClass")) {
        if (!templateDecl || !Result.Context->getSourceManager().isWrittenInMainFile(templateDecl->getLocation()))
            return;
        if (templateDecl->isLambda() || !templateDecl->isCompleteDefinition())
            return;
        if (templateDecl->getTemplateInstantiationPattern()) {
            if (!nestedTemplateDeclared) {
                SourceLocation startLoc = SM.getLocForStartOfFile(SM.getMainFileID());
                Rewrite.InsertText(startLoc, "template <int> class NestedTemplate;\n");
                nestedTemplateDeclared = true;
            }
            auto originalText = stringutils::rangetoStr(*Result.SourceManager, templateDecl->getSourceRange());
            size_t openBracePos = originalText.find("{");
            if (openBracePos != std::string::npos) {
                std::string insertion = "  struct {\n    NestedTemplate<1> field;\n  } member;\n";
                originalText.insert(openBracePos + 1, insertion);
                Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(templateDecl->getSourceRange()), originalText);
            }
        }
    }
}

void MutatorFrontendAction_389::MutatorASTConsumer_389::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher forwardMatcher = cxxRecordDecl(unless(clang::ast_matchers::hasAnyBase())).bind("classNoBase");
    DeclarationMatcher nestedMatcher = cxxRecordDecl(hasDescendant(templateTypeParmDecl())).bind("templateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(forwardMatcher, &callback);
    matchFinder.addMatcher(nestedMatcher, &callback);
    matchFinder.matchAST(Context);
}