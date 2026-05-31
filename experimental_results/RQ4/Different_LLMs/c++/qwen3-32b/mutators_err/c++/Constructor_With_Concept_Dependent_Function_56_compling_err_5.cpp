//header file
#pragma once
#include "Mutator_base.h"

/**
 * Constructor_With_Concept_Dependent_Function_56
 */ 
class MutatorFrontendAction_56 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(56)

private:
    class MutatorASTConsumer_56 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_56(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::set<std::string> processedClasses;
    };
};

//source file
#include "../include/Constructor_With_Concept_Dependent_Function_56.h"

// ========================================================================================================
#define MUT56_OUTPUT 1

void MutatorFrontendAction_56::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *constructor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constructor")) {
        if (!constructor || !Result.Context->getSourceManager().isWrittenInMainFile(constructor->getLocation()))
            return;

        auto *recordDecl = cast<clang::CXXRecordDecl>(constructor->getParent());
        std::string className = recordDecl->getNameAsString();
        if (processedClasses.count(className)) return;
        processedClasses.insert(className);

        if (constructor->getNumCtorInitializers() == 0) return;
        auto *initializer = constructor->getCtorInitializers()[0];
        std::string memberName;
        if (auto *memInit = dyn_cast<clang::CXXCtorInitializer>(initializer)) {
            if (memInit->isMemberInitializer()) {
                memberName = memInit->getMember()->getNameAsString();
            }
        }
        if (memberName.empty()) return;

        std::string conceptCode = "template<typename T>\nconcept HasValid = valid_function(T());\n";
        std::string validFunctionCode = "bool valid_function(" + className + " const &outer) {\n  return valid_function(outer." + memberName + ");\n}\n";

        SourceLocation classLoc = recordDecl->getLocation();
        Rewrite.InsertTextBefore(classLoc, conceptCode + validFunctionCode);

        std::string originalConstructor = stringutils::rangetoStr(*Result.SourceManager, constructor->getSourceRange());
        std::string modifiedConstructor = "template<typename U>\nrequires HasValid<U>\n" + originalConstructor;

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(constructor->getSourceRange()), modifiedConstructor);
    }
}

void MutatorFrontendAction_56::MutatorASTConsumer_56::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(hasAnyConstructorInitializer(anything())).bind("constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}