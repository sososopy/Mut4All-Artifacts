//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_With_Dependent_Type_178
 */ 
class MutatorFrontendAction_178 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(178)

private:
    class MutatorASTConsumer_178 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_178(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Parameter_With_Dependent_Type_178.h"

// ========================================================================================================
#define MUT178_OUTPUT 1

void MutatorFrontendAction_178::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpec")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getTemplateNameLoc()))
            return;

        if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
            if (!SD->isCompleteDefinition())
                return;

            std::string structName = SD->getNameAsString();
            auto templateArgs = MT->template_arguments();
            if (templateArgs.size() > 0) {
                std::string newTemplateParam = "typename T";
                std::string originalType = templateArgs[0].getAsType().getAsString();
                std::string replacement = "__make_integer_seq<" + newTemplateParam + ", int, 42>";

                std::string newStructDecl = "template <" + newTemplateParam + "> struct " + structName + " { " + replacement + " m; };";
                std::string instantiation = structName + "<" + originalType + ">";

                SourceLocation startLoc = SD->getBeginLoc();
                SourceLocation endLoc = SD->getEndLoc().getLocWithOffset(1);
                Rewrite.ReplaceText(SourceRange(startLoc, endLoc), newStructDecl);
                Rewrite.ReplaceText(MT->getSourceRange(), instantiation);
            }
        }
    }
}

void MutatorFrontendAction_178::MutatorASTConsumer_178::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher structMatcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    TypeMatcher templateSpecMatcher = templateSpecializationType().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.addMatcher(templateSpecMatcher, &callback);
    matchFinder.matchAST(Context);
}