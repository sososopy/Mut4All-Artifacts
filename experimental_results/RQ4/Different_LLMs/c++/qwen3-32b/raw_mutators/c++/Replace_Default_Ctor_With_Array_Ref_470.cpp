//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Ctor_With_Array_Ref_470
 */ 
class MutatorFrontendAction_470 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(470)

private:
    class MutatorASTConsumer_470 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_470(Rewriter &R) : TheRewriter(R) {}
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
        // Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Default_Ctor_With_Array_Ref_470.h"

// ========================================================================================================
#define MUT470_OUTPUT 1

void MutatorFrontendAction_470::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassWithDefaultCtorAndArray")) {
        // Filter nodes in header files
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        // Find the first array member
        clang::FieldDecl *arrayMember = nullptr;
        for (auto *field : CD->fields()) {
            if (field->getType()->isArrayType()) {
                arrayMember = field;
                break;
            }
        }
        if (!arrayMember)
            return;

        // Get the array type and size
        clang::QualType arrayType = arrayMember->getType();
        if (auto *arrayTy = arrayType->getAs<clang::ArrayType>()) {
            clang::QualType elementType = arrayTy->getElementType();
            unsigned size = 0;
            if (auto *constArrayTy = dyn_cast<clang::ConstantArrayType>(arrayTy)) {
                size = constArrayTy->getLength().getZExtValue();
            } else {
                return; // Skip if not constant size
            }

            // Generate the new reference member
            std::string refMemberName = "arrRef";
            std::string elementTypeStr = elementType.getAsString(CD->getASTContext().getPrintingPolicy());
            std::string refMemberDecl = elementTypeStr + "(&" + refMemberName + ")[" + std::to_string(size) + "];";

            // Insert the new member into the class
            SourceLocation insertLoc = CD->getEndLoc().getLocWithOffset(-1); // Before the closing brace
            Rewrite.InsertText(insertLoc, "\n    " + refMemberDecl, true, true);

            // Find the default constructor
            for (auto *ctor : CD->ctors()) {
                if (ctor->isDefaultConstructor()) {
                    // Get the constructor's source range
                    SourceRange ctorRange = ctor->getSourceRange();
                    std::string ctorText = stringutils::rangetoStr(*Result.SourceManager, ctorRange);

                    // Generate the new initializer
                    std::string newInit = refMemberName + "(*new " + elementTypeStr + "[1][" + std::to_string(size) + "])";

                    // Replace the constructor's body with the new initializer
                    std::string newCtorText = ctorText.substr(0, ctorText.find("{")) + " : " + newInit + " {\n    }\n";
                    Rewrite.ReplaceText(ctorRange, newCtorText);
                    break;
                }
            }
        }
    }
}
  
void MutatorFrontendAction_470::MutatorASTConsumer_470::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(
        has(cxxConstructorDecl(isDefaultConstructor())),
        hasDescendant(fieldDecl(hasType(arrayType())))
    ).bind("ClassWithDefaultCtorAndArray");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}