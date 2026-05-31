//source file
#include "../include/add_virtual_base_class_236.h"

// ========================================================================================================
#define MUT236_OUTPUT 1

void MutatorFrontendAction_236::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDecl")) {
        if (!recordDecl)
            return;
        if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc()))
            return;
        if (!recordDecl->isCompleteDefinition() || recordDecl->isLambda())
            return;

        if (visitedClasses.find(recordDecl) == visitedClasses.end()) {
            visitedClasses.insert(recordDecl);

            std::string className = recordDecl->getNameAsString();
            if (className.empty())
                return;

            std::string virtualBaseClass = "VirtualBase_" + className;
            std::string baseClassDefinition = "class " + virtualBaseClass + " {};\n";
            std::string derivedClassModification = " : virtual public " + virtualBaseClass;

            // Insert the virtual base class definition before the current class
            Rewrite.InsertTextBefore(recordDecl->getBeginLoc(), baseClassDefinition);

            // Modify the current class to inherit from the virtual base class
            Rewrite.InsertTextAfterToken(recordDecl->getBeginLoc(), derivedClassModification);
        }
    }
}

void MutatorFrontendAction_236::MutatorASTConsumer_236::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("recordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}