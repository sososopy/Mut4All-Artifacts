//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Capture_Nesting_With_This_497
 */ 
class MutatorFrontendAction_497 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(497)

private:
    class MutatorASTConsumer_497 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_497(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Lambda_Capture_Nesting_With_This_497.h"

// ========================================================================================================
#define MUT497_OUTPUT 1

void MutatorFrontendAction_497::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *fieldDecl = Result.Nodes.getNodeAs<clang::CXXFieldDecl>("Field")) {
        if (!fieldDecl || !Result.Context->getSourceManager().isWrittenInMainFile(fieldDecl->getLocation()))
            return;

        clang::ASTContext &Context = *Result.Context;
        SourceManager &SM = Context.getSourceManager();
        const LangOptions &LO = Context.getLangOpts();
        Rewriter &Rewriter = this->Rewrite;

        // Get the field's type
        QualType fieldType = fieldDecl->getType();
        std::string fieldTypeStr = fieldType.getAsString();

        // Get the initializer as a LambdaExpr
        Expr *init = fieldDecl->getInClassInitializer();
        if (!init)
            return;

        if (auto *lambda = dyn_cast<clang::LambdaExpr>(init)) {
            // Get the source code of the lambda
            SourceLocation start = lambda->getBeginLoc();
            SourceLocation end = lambda->getEndLoc();
            std::string lambdaCode = stringutils::rangetoStr(SM, CharSourceRange::getTokenRange(SourceRange(start, end)));

            // Generate struct name
            std::string structName = "MutatorNestedStruct_497";

            // Extract the body of the lambda
            size_t openParen = lambdaCode.find('(');
            size_t closeBrace = lambdaCode.rfind('}');
            std::string body;
            if (openParen != std::string::npos && closeBrace != std::string::npos) {
                body = lambdaCode.substr(openParen + 1, closeBrace - openParen - 1);
            } else {
                body = "";
            }

            // Generate the struct code
            std::string structCode = "  struct " + structName + " {\n    " + fieldTypeStr + " func = [this](){ " + body + " };\n  };\n";

            // Insert the struct into the class
            SourceLocation insertLoc = fieldDecl->getBeginLoc();
            Rewriter.InsertText(insertLoc, structCode, true, true);

            // Modify the field's type to std::function<StructName(int)>
            std::string newFieldType = "std::function<" + structName + "(int)>";
            SourceRange typeRange = fieldDecl->getTypeSourceInfo()->getTypeLoc().getSourceRange();
            Rewriter.ReplaceText(typeRange, newFieldType);

            // Generate the outer lambda
            std::string outerLambda = "[this](auto) { return " + structName + "{.func = [this](){ " + body + " }}; }";

            // Replace the field's initializer
            SourceRange initRange = lambda->getSourceRange();
            Rewriter.ReplaceText(initRange, outerLambda);
        }
    }
}
  
void MutatorFrontendAction_497::MutatorASTConsumer_497::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxFieldDecl(
        hasType(templateSpecializationType(
            hasDeclaration(classTemplateDecl(hasName("std::function"))))),
        hasInitializer(ignoringImplicit(lambdaExpr()))
    ).bind("Field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}