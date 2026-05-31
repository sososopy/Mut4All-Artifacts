//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Template_Function_Within_Template_Class_186
 */ 
class MutatorFrontendAction_186 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(186)

private:
    class MutatorASTConsumer_186 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_186(Rewriter &R) : TheRewriter(R) {}
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
        const CXXRecordDecl *currentClass = nullptr;
        const FunctionTemplateDecl *currentMethod = nullptr;
    };
};

//source file
#include "../include/Specialize_Template_Function_Within_Template_Class_186.h"

// ========================================================================================================
#define MUT186_OUTPUT 1

void MutatorFrontendAction_186::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *ClassTemplate = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!ClassTemplate || !Result.Context->getSourceManager().isWrittenInMainFile(ClassTemplate->getLocation()))
            return;

        currentClass = ClassTemplate->getTemplatedDecl();
    } else if (auto *MethodTemplate = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateMethod")) {
        if (!MethodTemplate || !Result.Context->getSourceManager().isWrittenInMainFile(MethodTemplate->getLocation()))
            return;

        currentMethod = MethodTemplate;

        if (currentClass && currentMethod) {
            // Get the source code text of target node
            auto className = currentClass->getNameAsString();
            auto methodName = currentMethod->getNameAsString();
            std::string specialization = "template <>\n";
            specialization += "template <typename T2>\n";
            specialization += "void " + className + "<SomeTemplate>::" + methodName + "(SomeTemplate<T2>) {}\n";

            // Perform mutation on the source code text by applying string replacement
            Rewrite.InsertTextAfter(currentClass->getEndLoc(), "\n/*mut186*/" + specialization);
        }
    }
}
  
void MutatorFrontendAction_186::MutatorASTConsumer_186::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher classMatcher = classTemplateDecl(has(cxxRecordDecl(hasMethod(functionTemplateDecl())))).bind("TemplateClass");
    DeclarationMatcher methodMatcher = functionTemplateDecl().bind("TemplateMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.matchAST(Context);
}