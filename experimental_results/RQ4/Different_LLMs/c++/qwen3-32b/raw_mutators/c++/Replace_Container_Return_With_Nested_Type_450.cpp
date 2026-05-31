//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Container_Return_With_Nested_Type_450
 */ 
class MutatorFrontendAction_450 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(450)

private:
    class MutatorASTConsumer_450 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_450(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Container_Return_With_Nested_Type_450.h"

// ========================================================================================================
#define MUT450_OUTPUT 1

void MutatorFrontendAction_450::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("func")) {
        //Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        const auto *enclosingClass = MT->getParent();
        if (!enclosingClass || !enclosingClass->isClassTemplate())
            return;

        std::string className = enclosingClass->getNameAsString();
        if (className.empty())
            return;

        QualType returnType = MT->getReturnType();
        if (const auto *TST = returnType->getAs<clang::TemplateSpecializationType>()) {
            const auto *decl = TST->getTemplateName().getAsTemplateDecl();
            if (!decl)
                return;
            std::string containerName = decl->getNameAsString();
            if (containerName != "array" && containerName != "vector")
                return;

            SourceLocation returnTypeLoc = MT->getTrailingReturnTypeLoc();
            if (returnTypeLoc.isInvalid())
                return;

            CharSourceRange range = MT->getTrailingReturnTypeSourceRange();
            std::string originalReturnType = stringutils::rangetoStr(*Result.SourceManager, range);

            size_t openAngle = originalReturnType.find('<');
            size_t closeAngle = originalReturnType.rfind('>');
            if (openAngle == std::string::npos || closeAngle == std::string::npos)
                return;

            std::string templateArgs = originalReturnType.substr(openAngle + 1, closeAngle - openAngle - 1);
            std::vector<std::string> args = stringutils::split(templateArgs, ',');
            if (args.size() < 2)
                return;

            std::string secondArg = args[1];
            std::string newReturnType = "typename " + className + "::Nested<" + className + "::Tag, " + secondArg + ">::type";

            Rewrite.ReplaceText(range, newReturnType);
        }
    }
}
  
void MutatorFrontendAction_450::MutatorASTConsumer_450::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        hasTrailingReturnType(),
        ofClass(classTemplateDecl().bind("enclosingClass"))
    ).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}