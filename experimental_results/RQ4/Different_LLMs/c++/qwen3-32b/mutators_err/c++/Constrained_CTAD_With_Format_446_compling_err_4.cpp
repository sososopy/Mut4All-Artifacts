//header file
#pragma once
#include "Mutator_base.h"

/**
 * Constrained_CTAD_With_Format_446
 */ 
class MutatorFrontendAction_446 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(446)
private:
    class MutatorASTConsumer_446 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_446(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> modified_classes;
    };
};

//source file
#include "../include/Constrained_CTAD_With_Format_446.h"

// ========================================================================================================
#define MUT446_OUTPUT 1

void MutatorFrontendAction_446::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;
        auto *CD = CT->getTemplatedDecl();
        if (!CD || !CD->isCompleteDefinition())
            return;
        std::string className = CT->getNameAsString();
        if (className.empty())
            return;
        SourceLocation endLoc = CD->getEndLoc();
        if (endLoc.isInvalid())
            return;
        std::string constructorCode = R"(
            template<typename T>
            requires std::integral<T>
            )";
        constructorCode += className + "(T val) : value(val) {}\n";
        Rewrite.InsertText(endLoc, constructorCode);
        modified_classes.insert(className);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        QualType returnType = FD->getReturnType();
        if (!returnType.getTypePtr() || !returnType->isRecordType())
            return;
        const RecordType *RT = returnType->getAs<RecordType>();
        const CXXRecordDecl *RD = dyn_cast<CXXRecordDecl>(RT->getDecl());
        if (!RD)
            return;
        std::string className = RD->getNameAsString();
        if (modified_classes.find(className) == modified_classes.end())
            return;
        if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("returnStmt")) {
            SourceRange range = RS->getSourceRange();
            if (range.isInvalid())
                return;
            std::string returnExpr = stringutils::rangetoStr(*Result.SourceManager, range);
            std::string newCode = "auto w = " + returnExpr + ";\n";
            newCode += "std::string result = std::format(\"{}\", w);\n";
            newCode += "return w;\n";
            Rewrite.ReplaceText(range, newCode);
        }
    }
}
  
void MutatorFrontendAction_446::MutatorASTConsumer_446::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher classMatcher = classTemplateDecl().bind("ClassTemplate");
    DeclarationMatcher functionMatcher = functionDecl(
        hasReturnType(type::isTemplateInstantiation()),
        hasDescendant(returnStmt().bind("returnStmt"))
    ).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}